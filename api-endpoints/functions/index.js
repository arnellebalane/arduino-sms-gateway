const functions = require('firebase-functions');
const { defineSecret } = require('firebase-functions/params');
const logger = require('firebase-functions/logger');
const admin = require('firebase-admin');
const axios = require('axios');

admin.initializeApp();

/**
 * Needed in order to call the Firebase Auth REST API. Normally you don't need to define app configurations in
 * Firebase Cloud Functions since those values are already available in the environment.
 */
const FIREBASE_API_KEY = defineSecret('FIREBASE_API_KEY');

/**
 * Ideally we want to have functions/endpoints that perform one thing only, but since this is going to be called from
 * a microcontroller device we want to minimize the number of HTTP requests that it makes. Since our use-case is simple
 * we can do both authentication and saving records to the database within the same function.
 */
exports.receiveMessage = functions
  .runWith({ secrets: [FIREBASE_API_KEY] })
  .https.onRequest(async (request, response) => {
    /**
     * Expected request payload type:
     *  {
     *    email: string;
     *    password: string;
     *    payload: any;
     *  }
     */
    const { email, password, payload } = request.body;

    /**
     * The Firebase Admin SDK does not support signing in using a user's credentials. Alternatively, we could use the
     * Firebase JavaScript (NodeJS) SDK to perform the authentication, but the REST API ended up being the easier
     * option for our use-case. https://firebase.google.com/docs/reference/rest/auth
     */
    const signInEndpoint = `https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=${FIREBASE_API_KEY.value()}`;

    try {
      /**
       * Attempt to sign-in using the user's credentials. We don't care about the response of the request, if it
       * succeeds then it is a valid user and we proceed with function execution.
       */
      const signInPayload = { email, password, returnSecureToken: true };
      await axios.post(signInEndpoint, signInPayload, {
        headers: {
          'Content-Type': 'application/json',
        },
      });
    } catch (error) {
      return response.status(403).end();
    }

    try {
      const doc = await admin.firestore().collection('messages').add(payload);
      logger.info(`New message stored: ${doc.id}`);
      response.status(200).end();
    } catch (error) {
      return response.status(500).end();
    }

    return response.status(200).end();
  });
