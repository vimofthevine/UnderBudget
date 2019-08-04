import { configureStore, getDefaultMiddleware } from 'redux-starter-kit';
import { apiMiddleware } from 'redux-api-middleware';
import { connectRouter, routerMiddleware } from 'connected-react-router';
import { snackbarReducer as snackbar } from 'material-ui-snackbar-redux';
import auth from './ducks/auth';
import ledgers from './ducks/ledgers';
import nav from './ducks/nav';
import history from './history';

const store = configureStore({
  reducer: {
    auth,
    ledgers,
    nav,
    router: connectRouter(history),
    snackbar,
  },
  middleware: [...getDefaultMiddleware(), apiMiddleware, routerMiddleware(history)],
});

export default store;
