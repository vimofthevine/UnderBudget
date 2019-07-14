import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import Dialog from './Dialog';

const DialogDemo = (props) => {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);
  const handleSubmit = action('submit');

  return (
    <Fragment>
      <Dialog
        actionText='Do it'
        onClose={handleClose}
        onSubmit={handleSubmit}
        open={open}
        title='Demo Dialog'
        {...props}
      >
        <p>Hi</p>
      </Dialog>
      <button type='button' onClick={handleOpen}>Open</button>
    </Fragment>
  );
};

storiesOf('Layout|Dialog', module)
  .add('default', () => (
    <DialogDemo />
  ))
  .add('mobile', () => (
    <DialogDemo fullScreen />
  ))
  .add('pending', () => (
    <DialogDemo open pending />
  ))
  .add('mobile pending', () => (
    <DialogDemo fullScreen open pending />
  ));
