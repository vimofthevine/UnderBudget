import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { Provider } from 'react-redux';
import FullNavPage from '../../components/FullNavPage/FullNavPage';
import Ledgers from './Ledgers';
import { createLedger } from '../../state/ducks/ledgers';

const makeStore = ({
  error = null,
  isLoading = false,
  isCreatePending = false,
  isUpdatePending = false,
  isCreateOpen = false,
  isModifyOpen = false,
  byId = {},
  allIds = [],
  modifyLedgerId = null,
  selectedLedgerId = null,
}) => ({
  getState: () => ({
    ledgers: {
      error,
      isLoading,
      isCreatePending,
      isUpdatePending,
      isCreateOpen,
      isModifyOpen,
      byId,
      allIds,
      modifyLedgerId,
      selectedLedgerId,
    },
    nav: {
      isDrawerOpen: false,
    },
  }),
  subscribe: () => 0,
  dispatch: action('dispatch'),
});

const createLedgers = (num) => {
  const allIds = [];
  const byId = {};
  for (let i = 0; i < num; i += 1) {
    const id = `00000000-0000-0000-0000-0000000000${i}`;
    allIds.push(id);
    byId[id] = {
      id,
      name: `Ledger ${i}`,
      defaultCurrency: 'USD',
      archived: false,
      created: '2019-07-11',
      owner: {
        id: 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa',
        name: 'John Smith',
      },
    };
  }
  return {
    allIds,
    byId,
  };
};

storiesOf('Ledger|Ledgers/Ledgers Page', module)
  .add('loading', () => (
    <Provider
      store={makeStore({ isLoading: true })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('load error', () => (
    <Provider
      store={makeStore({ error: 'Error loading ledgers' })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('loaded', () => (
    <Provider
      store={makeStore({
        ...createLedgers(2),
        selectedLedgerId: '00000000-0000-0000-0000-00000000001',
      })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('loaded with pagination', () => (
    <Provider
      store={makeStore({ ...createLedgers(20) })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('create dialog open', () => (
    <Provider
      store={makeStore({ ...createLedgers(2), isCreateOpen: true })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('create error', () => (
    <Provider
      store={makeStore({
        ...createLedgers(2),
        isCreateOpen: true,
        error: 'Error creating ledger',
      })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('edit dialog open', () => (
    <Provider
      store={makeStore({
        ...createLedgers(2),
        isModifyOpen: true,
        modifyLedgerId: '00000000-0000-0000-0000-00000000001',
      })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ))
  .add('edit error', () => (
    <Provider
      store={makeStore({
        ...createLedgers(2),
        isModifyOpen: true,
        modifyLedgerId: '00000000-0000-0000-0000-00000000001',
        error: 'Error modifying ledger',
      })}
    >
      <FullNavPage>
        <Ledgers />
      </FullNavPage>
    </Provider>
  ));
