import { configureStore, getDefaultMiddleware } from 'redux-starter-kit';
import { apiMiddleware } from 'redux-api-middleware';
import { snackbarReducer } from 'material-ui-snackbar-redux';
import navReducer from './ducks/nav';

const store = configureStore({
  reducer: {
    nav: navReducer,
    snackbar: snackbarReducer,
  },
  middleware: [...getDefaultMiddleware(), apiMiddleware],
});

export default store;
