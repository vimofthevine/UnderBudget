import { createReducer } from 'redux-starter-kit';
import decode from 'jwt-decode';
import * as types from './types';

const initialState = {
  error: null,

  loggedIn: false,
  username: null,
  token: null,
  decoded: null,
};

const reducer = createReducer(initialState, {
  [types.RECEIVE_LOGIN]: (state, action) => {
    state.loggedIn = true;
    state.username = action.payload.username;
    state.token = action.payload.token;
    state.decoded = decode(action.payload.token);
  },
  [types.FAILED_LOGIN]: (state) => {
    state.error = 'Login failed';
  },

  [types.FAILED_REGISTER]: (state) => {
    state.error = 'Registration failed';
  },

  [types.RECEIVE_LOGOUT]: (state) => {
    state.loggedIn = false;
    state.username = null;
    state.token = null;
    state.decoded = null;
  },

  [types.DISMISS_AUTH_ERROR]: (state) => {
    state.error = null;
  },
});
export default reducer;
