import React from 'react';
import { storiesOf } from '@storybook/react';
import LedgerListing from './LedgerListing';

const ledger1 = {
  id: 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx',
  name: 'My Ledger',
  currency: 'USD',
  created: '2019-07-09',
  owner: {
    id: 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa',
    name: 'John Smith',
  },
};
const ledger2 = {
  id: 'yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy',
  name: 'Your Ledger',
  currency: 'UAH',
  created: '2019-07-10',
  owner: {
    id: 'bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb',
    name: 'Billy Jean',
  },
};
const ledger3 = {
  id: 'zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz',
  name: 'Our Ledger',
  currency: 'USD',
  created: '2019-06-30',
  owner: {
    id: 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa',
    name: 'John Smith',
  },
};

const createLedgers = (num) => {
  const ledgers = [];
  for (let i = 0; i < num; i += 1) {
    ledgers.push({
      id: `00000000-0000-0000-0000-0000000000${i}`,
      name: `Ledger ${i}`,
      currency: 'USD',
      created: '2019-07-11',
      owner: {
        id: 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa',
        name: 'John Smith',
      },
    });
  }
  return ledgers;
};

storiesOf('Ledger|Ledgers/LedgerListing', module)
  .add('no ledgers', () => (
    <LedgerListing ledgers={[]} />
  ))
  .add('one ledger', () => (
    <LedgerListing ledgers={[ledger1]} />
  ))
  .add('multiple ledgers', () => (
    <LedgerListing ledgers={[ledger1, ledger2, ledger3]} />
  ))
  .add('5 ledgers', () => (
    <LedgerListing ledgers={createLedgers(5)} />
  ))
  .add('50 ledgers', () => (
    <LedgerListing ledgers={createLedgers(50)} />
  ));
