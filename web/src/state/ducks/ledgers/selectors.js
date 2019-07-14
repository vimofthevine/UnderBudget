import { createSelector } from 'reselect';

const getById = state => state.ledgers.byId;
const getAllIds = state => state.ledgers.allIds;
const getModifyLedgerId = state => state.ledgers.modifyLedgerId;
const getArchivedFilter = (state, props) => props.showArchived;

export const makeGetLedgers = () => createSelector(
  [getById, getAllIds, getArchivedFilter],
  (byId, allIds, showArchived) => {
    const ledgers = allIds.map(id => byId[id]);
    if (showArchived) {
      return ledgers;
    }
    return ledgers.filter(e => e.archived === false);
  },
);

export const makeGetModifyLedger = () => createSelector(
  [getById, getModifyLedgerId],
  (byId, modifyLedgerId) => byId[modifyLedgerId],
);
