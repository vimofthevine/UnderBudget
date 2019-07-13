import React from 'react';
import PropTypes from 'prop-types';
import { Formik } from 'formik';
import Dialog from '../Dialog/Dialog';
import LedgerForm from '../LedgerForm/LedgerForm';

const CreateLedgerDialog = ({ onClose, ...props }) => (
  <Formik
    initialValues={{ currency: 'USD', name: '' }}
    render={({ handleReset, handleSubmit }) => (
      <Dialog
        actionText='Create'
        title='Create Ledger'
        onSubmit={handleSubmit}
        onClose={() => {
          handleReset();
          onClose();
        }}
        {...props}
      >
        <LedgerForm />
      </Dialog>
    )}
  />
);

CreateLedgerDialog.propTypes = {
  onClose: PropTypes.func.isRequired,
};

export default CreateLedgerDialog;
