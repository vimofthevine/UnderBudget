import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Formik } from 'formik';
import * as yup from 'yup';
import Dialog from '../Dialog/Dialog';
import LedgerForm from '../LedgerForm/LedgerForm';
import { createLedger } from '../../state/ducks/ledgers';

const schema = yup.object().shape({
  name: yup.string().max(128, 'Too long').required('Required'),
  defaultCurrency: yup.string().required('Required'),
});

export const PureCreateLedgerDialog = ({
  onClose,
  onCreate,
  ...props
}) => (
  <Formik
    initialValues={{ defaultCurrency: 'USD', name: '' }}
    validationSchema={schema}
    onSubmit={(values) => {
      onCreate(values);
      onClose();
    }}
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

PureCreateLedgerDialog.propTypes = {
  onClose: PropTypes.func.isRequired,
  onCreate: PropTypes.func.isRequired,
};

const mapDispatchToProps = dispatch => ({
  onCreate: ledger => dispatch(createLedger(ledger)),
});

export default connect(null, mapDispatchToProps)(PureCreateLedgerDialog);
