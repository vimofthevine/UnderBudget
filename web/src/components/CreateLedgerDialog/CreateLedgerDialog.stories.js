import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureCreateLedgerDialog } from './CreateLedgerDialog';

const DialogDemo = (props) => {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  return (
    <Fragment>
      <PureCreateLedgerDialog
        onClose={handleClose}
        onCreate={action('create')}
        open={open}
        {...props}
      />
      <button type='button' onClick={handleOpen}>Open</button>
    </Fragment>
  );
};

storiesOf('Ledger|Ledgers/CreateLedgerDialog', module)
  .add('default', () => (
    <DialogDemo />
  ));
