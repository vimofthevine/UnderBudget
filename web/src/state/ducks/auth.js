import { createReducer } from 'redux-starter-kit';

const LOGIN = 'underbudget/auth/LOGIN';
const LOGOUT = 'underbudget/auth/LOGOUT';

const initialState = {
  loggedIn: false,
  username: null,
};

const reducer = createReducer(initialState, {
  [LOGIN]: (nextState, action) => {
    nextState.loggedIn = true;
    nextState.username = action.payload.username;
  },
  [LOGOUT]: (nextState) => {
    nextState.loggedIn = false;
  },
});
export default reducer;

export const login = payload => ({
  type: LOGIN,
  payload,
});

export const logout = () => ({
  type: LOGOUT,
});
