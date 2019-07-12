import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import CreateLedgerDialog from './CreateLedgerDialog';

const DialogDemo = () => {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  return (
    <Fragment>
      <CreateLedgerDialog
        onClose={handleClose}
        open={open}
      >
        <p>Hi</p>
      </CreateLedgerDialog>
      <button type='button' onClick={handleOpen}>Open</button>
    </Fragment>
  );
};

storiesOf('Ledger|Ledgers/CreateLedgerDialog', module)
  .add('default', () => (
    <DialogDemo />
  ));
