import { createReducer } from 'redux-starter-kit';
import decode from 'jwt-decode';
import * as types from './types';

const initialState = {
  loggedIn: false,
  username: null,
  token: null,
  decoded: null,
};

const reducer = createReducer(initialState, {
  [types.RECEIVE_LOGIN]: (nextState, action) => {
    nextState.loggedIn = true;
    nextState.username = action.payload.username;
    nextState.token = action.payload.token;
    nextState.decoded = decode(action.payload.token);
  },
  [types.RECEIVE_LOGOUT]: (nextState) => {
    nextState.loggedIn = false;
    nextState.username = null;
    nextState.token = null;
    nextState.decoded = null;
  },
});
export default reducer;
