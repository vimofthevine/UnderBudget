import { RSAA } from 'redux-api-middleware';
import * as types from './types';
import { withAuth } from './utils';

export const login = user => ({
  [RSAA]: {
    endpoint: '/tokens',
    method: 'POST',
    body: JSON.stringify(user),
    types: [types.REQUEST_LOGIN, types.RECEIVE_LOGIN, types.FAILED_LOGIN],
  },
});

export const logout = token => withAuth(({ headers }) => ({
  [RSAA]: {
    endpoint: `/tokens/${token.jti}`,
    method: 'DELETE',
    headers,
    types: [types.REQUEST_LOGOUT, types.RECEIVE_LOGOUT, types.FAILED_LOGOUT],
  },
}));

export const register = user => withAuth(({ headers }) => ({
  [RSAA]: {
    endpoint: '/users',
    method: 'POST',
    headers,
    body: JSON.stringify(user),
    types: [types.REQUEST_REGISTER, types.RECEIVE_REGISTER, types.FAILED_REGISTER],
  },
}));
