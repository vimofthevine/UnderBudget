import { configureStore, getDefaultMiddleware } from 'redux-starter-kit';
import { apiMiddleware } from 'redux-api-middleware';
import { connectReducer, routerMiddleware } from 'connected-react-router';
import { snackbarReducer } from 'material-ui-snackbar-redux';
import navReducer from './ducks/nav';
import history from './history';

const store = configureStore({
  reducer: {
    nav: navReducer,
    router: connectReducer(history),
    snackbar: snackbarReducer,
  },
  middleware: [...getDefaultMiddleware(), apiMiddleware, routerMiddleware(history)],
});

export default store;
