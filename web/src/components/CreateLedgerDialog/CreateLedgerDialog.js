import React from 'react';
import { Formik } from 'formik';
import Dialog from '../Dialog/Dialog';
import LedgerForm from '../LedgerForm/LedgerForm';

const CreateLedgerDialog = props => (
  <Formik
    initialValues={{ currency: 'USD' }}
    render={({ handleSubmit }) => (
      <Dialog
        actionText='Create'
        title='Create Ledger'
        onSubmit={handleSubmit}
        {...props}
      >
        <LedgerForm />
      </Dialog>
    )}
  />
);

export default CreateLedgerDialog;
