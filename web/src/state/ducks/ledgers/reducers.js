import { createReducer } from 'redux-starter-kit';
import * as types from './types';

const initialState = {
  error: null,

  isLoading: false,
  isCreatePending: false,
  isUpdatePending: false,

  isCreateOpen: false,
  isModifyOpen: false,

  byId: {},
  allIds: [],

  modifyLedgerId: null,
  selectedLedgerId: null,
};

const reducer = createReducer(initialState, {
  [types.REQUEST_FETCH_LEDGERS]: (state) => {
    state.isLoading = true;
  },
  [types.RECEIVE_FETCH_LEDGERS]: (state, action) => {
    state.isLoading = false;
    state.byId = action.payload.ledgers;
    state.allIds = Object.keys(state.byId);
  },
  [types.FAILED_FETCH_LEDGERS]: (state) => {
    state.isLoading = false;
    state.error = 'Error loading ledgers';
  },

  [types.REQUEST_CREATE_LEDGER]: (state) => {
    state.isCreatePending = true;
  },
  [types.RECEIVE_CREATE_LEDGER]: (state, action) => {
    state.isCreatePending = false;
    state.isCreateOpen = false;
    state.byId[action.payload.id] = action.payload;
    state.allIds.push(action.payload.id);
  },
  [types.FAILED_CREATE_LEDGER]: (state) => {
    state.isCreatePendingea = false;
    state.error = 'Error creating ledger';
  },

  [types.REQUEST_MODIFY_LEDGER]: (state) => {
    state.isUpdatePending = true;
  },
  [types.RECEIVE_MODIFY_LEDGER]: (state, action) => {
    state.isUpdatePending = true;
    state.isModifyOpen = true;
    state.byId[action.payload.id] = action.payload;
  },
  [types.FAILED_MODIFY_LEDGER]: (state) => {
    state.isUpdatePending = false;
    state.error = 'Error modifying ledger';
  },

  [types.SHOW_CREATE_LEDGER]: (state) => {
    state.isCreateOpen = true;
  },
  [types.HIDE_CREATE_LEDGER]: (state) => {
    state.isCreateOpen = false;
  },

  [types.SHOW_MODIFY_LEDGER]: (state, action) => {
    state.isModifyOpen = true;
    state.modifyLedgerId = action.payload;
  },
  [types.HIDE_MODIFY_LEDGER]: (state) => {
    state.isModifyOpen = false;
  },

  [types.DISMISS_LEDGER_ERROR]: (state) => {
    state.error = null;
  },

  [types.SELECT_LEDGER]: (state, action) => {
    state.selectedLedgerId = action.payload;
  },
});

export default reducer;
