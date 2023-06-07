const functions = require('firebase-functions');
const { defineSecret } = require('firebase-functions/params');
const logger = require('firebase-functions/logger');
const admin = require('firebase-admin');
const axios = require('axios');

admin.initializeApp();

const FIREBASE_API_KEY = defineSecret('FIREBASE_API_KEY');

exports.receiveMessage = functions
  .runWith({ secrets: [FIREBASE_API_KEY] })
  .https.onRequest(async (request, response) => {
    const { email, password, payload } = request.body;
    const signInEndpoint = `https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=${FIREBASE_API_KEY.value()}`;

    try {
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
