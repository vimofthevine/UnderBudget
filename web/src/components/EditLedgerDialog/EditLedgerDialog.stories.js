import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { withKnobs, object } from '@storybook/addon-knobs';
import { action } from '@storybook/addon-actions';
import { PureEditLedgerDialog } from './EditLedgerDialog';

const DialogDemo = () => {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  return (
    <Fragment>
      <PureEditLedgerDialog
        isOpen={open}
        ledger={object('ledger', { id: 'id1', defaultCurrency: 'USD', name: 'Demo Ledger' })}
        onClose={handleClose}
        onUpdate={action('update')}
      />
      <button type='button' onClick={handleOpen}>Open</button>
    </Fragment>
  );
};

storiesOf('Ledger|Ledgers/EditLedgerDialog', module)
  .addDecorator(withKnobs)
  .add('default', () => (
    <DialogDemo />
  ));
