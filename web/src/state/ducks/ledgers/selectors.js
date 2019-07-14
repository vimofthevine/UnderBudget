import { createSelector } from 'reselect';

const getById = state => state.ledgers.byId;
const getAllIds = state => state.ledgers.allIds;
const getModifyLedgerId = state => state.ledgers.modifyLedgerId;

export const makeGetLedgers = () => createSelector(
  [getById, getAllIds],
  (byId, allIds) => allIds.map(id => byId[id]),
);

export const makeGetModifyLedger = () => createSelector(
  [getById, getModifyLedgerId],
  (byId, modifyLedgerId) => byId[modifyLedgerId],
);
