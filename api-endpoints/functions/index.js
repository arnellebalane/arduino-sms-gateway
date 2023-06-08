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
     * Requests to this function should be authorized using Basic auth. This means there should be an Authorization
     * header with value "Basic base64(email:password)"
     */
    const credential = getCredentials(request.headers['authorization']);
    try {
      await authenticateUser(credential);
    } catch (error) {
      logger.error(error, { structuredClone: true });
      logger.debug('Invalid user credentials.');
      return response.status(403).end();
    }

    try {
      const doc = await admin.firestore().collection('messages').add(request.body);
      logger.info(`New message stored: ${doc.id}`);
      response.status(200).end();
    } catch (error) {
      logger.debug(error, { structuredClone: true });
      return response.status(500).end();
    }

    return response.status(200).end();
  });

function getCredentials(authorization) {
  if (!authorization) {
    logger.debug('Authorization header not found.');
    return response.status(403).send();
  }
  const [type, token] = authorization?.split(' ') ?? [];
  if (type !== 'Basic') {
    logger.debug(`Authorization type "${type}" is not supported.`);
    return response.status(403).send();
  }
  const buffer = Buffer.from(token, 'base64');
  const [email, password] = buffer.toString('ascii').split(':');
  return { email, password };
}

async function authenticateUser({ email, password }) {
  /**
   * The Firebase Admin SDK does not support signing in using a user's credentials. Alternatively, we could use the
   * Firebase JavaScript (NodeJS) SDK to perform the authentication, but the REST API ended up being the easier option
   * for our use-case. https://firebase.google.com/docs/reference/rest/auth
   */
  const signInEndpoint = `https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=${FIREBASE_API_KEY.value()}`;

  /**
   * Attempt to sign-in using the user's credentials. We don't care about the response of the request, if it succeeds
   * then it is a valid user and we proceed with function execution.
   */
  const signInPayload = { email, password, returnSecureToken: true };
  await axios.post(signInEndpoint, signInPayload, {
    headers: {
      'Content-Type': 'application/json',
    },
  });
}
