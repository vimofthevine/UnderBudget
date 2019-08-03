import { createReducer } from 'redux-starter-kit';
import { RSAA } from 'redux-api-middleware';
import decode from 'jwt-decode';

const REQUEST_LOGIN = 'underbudget/auth/REQUEST_LOGIN';
const RECEIVE_LOGIN = 'underbudget/auth/RECEIVE_LOGIN';
const FAILED_LOGIN = 'underbudget/auth/FAILED_LOGIN';

const REQUEST_LOGOUT = 'underbudget/auth/REQUEST_LOGOUT';
const RECEIVE_LOGOUT = 'underbudget/auth/RECEIVE_LOGOUT';
const FAILED_LOGOUT = 'underbudget/auth/FAILED_LOGOUT';

const REQUEST_REGISTER = 'underbudget/auth/REQUEST_REGISTER';
const RECEIVE_REGISTER = 'underbudget/auth/RECEIVE_REGISTER';
const FAILED_REGISTER = 'underbudget/auth/FAILED_REGISTER';

const initialState = {
  loggedIn: false,
  username: null,
  token: null,
  decoded: null,
};

const reducer = createReducer(initialState, {
  [RECEIVE_LOGIN]: (nextState, action) => {
    nextState.loggedIn = true;
    nextState.username = action.payload.username;
    nextState.token = action.payload.token;
    nextState.decoded = decode(action.payload.token);
  },
  [RECEIVE_LOGOUT]: (nextState) => {
    nextState.loggedIn = false;
    nextState.username = null;
    nextState.token = null;
    nextState.decoded = null;
  },
});
export default reducer;

export const login = user => ({
  [RSAA]: {
    endpoint: '/tokens',
    method: 'POST',
    body: JSON.stringify(user),
    types: [REQUEST_LOGIN, RECEIVE_LOGIN, FAILED_LOGIN],
  },
});

export const withAuth = creator => (dispatch, getState) => {
  dispatch(creator({
    headers: {
      Authorization: `Bearer ${getState().auth.token}`,
    },
  }));
};

export const logout = token => withAuth(({ headers }) => ({
  [RSAA]: {
    endpoint: `/tokens/${token.jti}`,
    method: 'DELETE',
    headers,
    types: [REQUEST_LOGOUT, RECEIVE_LOGOUT, FAILED_LOGOUT],
  },
}));

export const register = user => withAuth(({ headers }) => ({
  [RSAA]: {
    endpoint: '/users',
    method: 'POST',
    headers,
    body: JSON.stringify(user),
    types: [REQUEST_REGISTER, RECEIVE_REGISTER, FAILED_REGISTER],
  },
}));
