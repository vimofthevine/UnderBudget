import { createReducer } from 'redux-starter-kit';
import * as types from './types';

const initialState = {
  loadError: false,
  createError: false,
  updateError: false,
  isLoading: false,
  isCreatePending: false,
  isUpdatePending: false,
  allLedgers: {},
  selectedLedger: null,
};

const reducer = createReducer(initialState, {
  [types.REQUEST_FETCH_LEDGERS]: (state) => {
    state.isLoading = true;
    state.loadError = false;
  },
  [types.RECEIVE_FETCH_LEDGERS]: (state, action) => {
    state.isLoading = false;
    state.allLedgers = action.payload.ledgers;
  },
  [types.FAILED_FETCH_LEDGERS]: (state) => {
    state.isLoading = false;
    state.loadError = true;
  },

  [types.REQUEST_CREATE_LEDGER]: (state) => {
    state.isCreatePending = true;
    state.createError = false;
  },
  [types.RECEIVE_CREATE_LEDGER]: (state, action) => {
    state.isCreatePending = false;
    state.allLedgers[action.payload.id] = action.payload;
  },
  [types.FAILED_CREATE_LEDGER]: (state) => {
    state.isCreatePendingea = false;
    state.createError = true;
  },

  [types.REQUEST_UPDATE_LEDGER]: (state) => {
    state.isUpdatePending = true;
    state.updateError = false;
  },
  [types.RECEIVE_UPDATE_LEDGER]: (state, action) => {
    state.isUpdatePending = true;
    state.allLedgers[action.payload.id] = action.payload;
  },
  [types.FAILED_UPDATE_LEDGER]: (state) => {
    state.isUpdatePending = false;
    state.updateError = true;
  },

  [types.SELECT_LEDGER]: (state, action) => {
    state.selectedLedger = state.allLedgers[action.payload];
  },
});

export default reducer;
