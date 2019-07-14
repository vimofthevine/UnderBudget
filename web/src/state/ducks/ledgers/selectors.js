import { createSelector } from 'reselect';
import { object } from 'prop-types';

const getById = state => state.ledgers.byId;

const getArchivedFilter = (state, props) => props.showArchived;

// eslint-disable-next-line import/prefer-default-export
export const makeGetLedgers = () => createSelector(
  [getById, getArchivedFilter],
  (byId, showArchived) => {
    if (showArchived) {
      return object.values(byId).filter(e => e.archived === showArchived);
    }
    return object.values(byId);
  },
);
