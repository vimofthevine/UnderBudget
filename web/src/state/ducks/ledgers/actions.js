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

export const updateLedger = ({ id, ...ledger }) => ({
  [RSAA]: {
    endpoint: `/ledgers/${id}`,
    method: 'PUT',
    body: JSON.stringify(ledger),
    types: [
      types.REQUEST_UPDATE_LEDGER,
      types.RECEIVE_UPDATE_LEDGER,
      types.FAILED_UPDATE_LEDGER,
    ],
  },
});

export const selectLedger = ({ id }) => ({
  type: types.SELECT_LEDGER,
  payload: id,
});
