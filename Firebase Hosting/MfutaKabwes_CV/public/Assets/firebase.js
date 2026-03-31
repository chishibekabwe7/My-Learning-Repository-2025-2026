// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBXvISov6SImcA_VwTLBRmdt1j_RzKXqKg",
  authDomain: "mfuta-kabwe-resume.firebaseapp.com",
  projectId: "mfuta-kabwe-resume",
  storageBucket: "mfuta-kabwe-resume.firebasestorage.app",
  messagingSenderId: "761026008227",
  appId: "1:761026008227:web:f96dde78119bcfb86b9b16",
  measurementId: "G-4PW59S5D61"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);