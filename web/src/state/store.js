import { configureStore, getDefaultMiddleware } from 'redux-starter-kit';
import { apiMiddleware } from 'redux-api-middleware';
import { connectReducer, routerMiddleware } from 'connected-react-router';
import { snackbarReducer as snackbar } from 'material-ui-snackbar-redux';
import ledgers from './ducks/ledgers';
import nav from './ducks/nav';
import history from './history';

const store = configureStore({
  reducer: {
    ledgers,
    nav,
    router: connectReducer(history),
    snackbar,
  },
  middleware: [...getDefaultMiddleware(), apiMiddleware, routerMiddleware(history)],
});

export default store;
