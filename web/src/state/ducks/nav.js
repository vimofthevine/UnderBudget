import { createReducer } from 'redux-starter-kit';

const OPEN_DRAWER = 'underbudget/drawer/OPEN_DRAWER';
const CLOSE_DRAWER = 'underbudget/drawer/CLOSE_DRAWER';

const initialState = {
  drawerIsOpen: false,
};

const reducer = createReducer(initialState, {
  [OPEN_DRAWER]: (nextState) => {
    nextState.drawerIsOpen = true;
  },
});
export default reducer;

export const openDrawer = () => ({
  type: OPEN_DRAWER,
});

export const closeDrawer = () => ({
  type: CLOSE_DRAWER,
});
