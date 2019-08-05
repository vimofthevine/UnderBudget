export const getAuthError = state => state.auth.error;
export const getIsLoginPending = state => state.auth.isLoginPending;
export const getIsRegisterPending = state => state.auth.isRegisterPending;
export const getIsLoggedIn = state => state.auth.loggedIn;
export const getUsername = state => state.auth.username;
export const getToken = state => state.auth.token;
export const getDecodedToken = state => state.auth.decoded;
