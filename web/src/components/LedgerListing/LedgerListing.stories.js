import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureLedgerListing as LedgerListing } from './LedgerListing';

const handleArchive = action('archive');
const handleEdit = action('edit');
const handleSelect = action('select');
const handleUnarchive = action('unarchive');

const ledger1 = {
  id: 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx',
  name: 'My Ledger',
  defaultCurrency: 'USD',
  created: '2019-07-09',
  owner: {
    id: 'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa',
    name: 'John Smith',
  },
};
const ledger2 = {
  id: 'yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy',
  name: 'Your Ledger',
  defaultCurrency: 'UAH',
  created: '2019-07-10',
  owner: {
    id: 'bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb',
    name: 'Billy Jean',
  },
};
const ledger3 = {
  id: 'zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz',
  name: 'Our Ledger',
  defaultCurrency: 'USD',
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
      defaultCurrency: 'USD',
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
  .add('loading ledgers', () => (
    <LedgerListing
      isLoading
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[]}
    />
  ))
  .add('no ledgers', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[]}
    />
  ))
  .add('one ledger', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[ledger1]}
      selectedLedgerId=''
    />
  ))
  .add('multiple ledgers', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[ledger1, ledger2, ledger3]}
      selectedLedgerId=''
    />
  ))
  .add('selected ledger', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[ledger1, ledger2, ledger3]}
      selectedLedgerId='yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy'
    />
  ))
  .add('with archived ledger', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={[ledger1, ledger2, { archived: true, ...ledger3 }]}
    />
  ))
  .add('5 ledgers', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={createLedgers(5)}
      selectedLedgerId=''
    />
  ))
  .add('50 ledgers', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      ledgers={createLedgers(50)}
      selectedLedgerId=''
    />
  ))
  .add('pagination above', () => (
    <LedgerListing
      onArchive={handleArchive}
      onEdit={handleEdit}
      onSelect={handleSelect}
      onUnarchive={handleUnarchive}
      paginationAbove
      ledgers={createLedgers(50)}
      selectedLedgerId=''
    />
  ));
