import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { withKnobs, object } from '@storybook/addon-knobs';
import EditLedgerDialog from './EditLedgerDialog';

const DialogDemo = () => {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  return (
    <Fragment>
      <EditLedgerDialog
        ledger={object('ledger', { defaultCurrency: 'USD', name: 'Demo Ledger' })}
        onClose={handleClose}
        open={open}
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
