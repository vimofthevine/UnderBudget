import { RSAA } from 'redux-api-middleware';
import * as types from './types';

export const fetchLedgers = () => ({
  [RSAA]: {
    endpoint: '/ledgers',
    method: 'GET',
    types: [
      types.REQUEST_FETCH_LEDGERS,
      types.RECEIVE_FETCH_LEDGERS,
      types.FAILED_FETCH_LEDGERS,
    ],
  },
});

export const createLedger = ledger => ({
  [RSAA]: {
    endpoint: '/ledgers',
    method: 'POST',
    body: JSON.stringify(ledger),
    types: [
      types.REQUEST_CREATE_LEDGER,
      types.RECEIVE_CREATE_LEDGER,
      types.FAILED_CREATE_LEDGER,
    ],
  },
});

export const modifyLedger = ({ id, ...ledger }) => ({
  [RSAA]: {
    endpoint: `/ledgers/${id}`,
    method: 'PUT',
    body: JSON.stringify(ledger),
    types: [
      types.REQUEST_MODIFY_LEDGER,
      types.RECEIVE_MODIFY_LEDGER,
      types.FAILED_MODIFY_LEDGER,
    ],
  },
});

export const showCreateLedger = () => ({
  type: types.SHOW_CREATE_LEDGER,
});

export const hideCreateLedger = () => ({
  type: types.HIDE_CREATE_LEDGER,
});

export const showModifyLedger = ({ id }) => ({
  type: types.SHOW_MODIFY_LEDGER,
  payload: id,
});

export const hideModifyLedger = () => ({
  type: types.HIDE_MODIFY_LEDGER,
});

export const dismissLedgerError = () => ({
  type: types.DISMISS_LEDGER_ERROR,
});

export const selectLedger = ({ id }) => ({
  type: types.SELECT_LEDGER,
  payload: id,
});
