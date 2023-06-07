# Arduino SMS Gateway - API Endpoints

This contains the code for the API endpoints used by the Arduino SMS Gateway project to receive and store SMS messages. The API endpoints are implemented as Firebase Cloud Functions.

## Prerequisites

We are going to need the following in order to setup and run this project:

1. A Firebase project
   1. Create a new project from the [Firebase Console](https://console.firebase.google.com/)
      1. Take note of the project ID for this Firebase project
   1. Enable billing for the project. This is a requirement in order to use Cloud Functions
   1. Create a Firestore database
      1. From the Firebase project sidebar, select **Build** > **Firestore Database**
      1. Click on the **Create database** button
      1. Select the default Firestore security rules. This can be changed later.
         1. We can start in **production mode** for now as this we don't need to access our data outside of Cloud Functions, but in the future if we want to add other ways to view our data (e.g. a website or mobile app) we can modify the security rules to allow those.
      1. Finish creating the Firestore database
   1. Enable the **Secret Manager API** for the Google Cloud project. We use this API to store configuration parameters.
      1. Visit the [Secrets Manager API](https://console.cloud.google.com/apis/library/secretmanager.googleapis.com) product page from the Google Cloud Platform console
      1. Make sure that the correct project is selected, then **Enable** the API
1. Firebase CLI
   1. Install the CLI: `npm install -g firebase-tools`
   1. Sign-in to your account: `firebase login`

## Setup development environment

1. Clone this repository and install dependencies

   ```bash
   git clone https://github.com/arnellebalane/arduino-sms-gateway
   cd arduino-sms-gateway/api-endpoints
   npm ci
   ```
1. Replace the project ID in `api-endpoints/.firebaserc` with your own Firebase project ID

## Deploy functions

```
firebase deploy --only functions
```

When running for the first time, the command will ask for a value for the `FIREBASE_API_KEY` secret.

1. Go to project dashboard in the [Firebase Console](https://console.firebase.google.com/)
1. Go to **Project settings**
1. Copy the value of **Web API Key** and use it as the `FIREBASE_API_KEY` value
1. Press enter to proceed with the deployment

## Create user accounts

1. Go to project dashboard in the [Firebase Console](https://console.firebase.google.com/)
1. From the Firebase project sidebar, select **Build** > **Authentication**
1. In the **Sign-in method** tab, enable the **Email/Password** sign-in provider
1. In the **Users** tab, add the users that should be allowed to use the API

## Sending requests to the functions

The `receiveMessage` function supports `POST` requests sent to it with the following payload:

```json
{
  "email": "[USER_EMAIL]",
  "password": "[USER_PASSWORD]",
  "payload": {
    "sender": "<the sender of the message>",
    "message": "<the contents of the message>"
  }
}
```

Make sure to use the email and password for a user account that has been created in the **Authentication** page.

## Viewing messages data

The messages that have been sent to the API and and stored in the database can be viewed from the **Firestore Database** page.

1. Go to project dashboard in the [Firebase Console](https://console.firebase.google.com/)
1. From the Firebase project sidebar, select **Build** > **Firestore Database**
