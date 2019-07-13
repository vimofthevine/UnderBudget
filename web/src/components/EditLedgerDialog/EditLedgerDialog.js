import React from 'react';
import PropTypes from 'prop-types';
import { Formik } from 'formik';
import Dialog from '../Dialog/Dialog';
import LedgerForm from '../LedgerForm/LedgerForm';

const EditLedgerDialog = ({ ledger, onClose, ...props }) => (
  <Formik
    initialValues={ledger}
    enableReinitialize
    render={({ handleReset, handleSubmit }) => (
      <Dialog
        actionText='Save'
        title='Modify Ledger'
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

EditLedgerDialog.propTypes = {
  ledger: PropTypes.shape({
    name: PropTypes.string,
    currency: PropTypes.string,
  }).isRequired,
  onClose: PropTypes.func.isRequired,
};

export default EditLedgerDialog;
