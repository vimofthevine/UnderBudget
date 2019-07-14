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
  isOpen,
  onClose,
  onCreate,
}) => (
  <Formik
    initialValues={{ defaultCurrency: 'USD', name: '' }}
    validationSchema={schema}
    onSubmit={onCreate}
    render={({ handleReset, handleSubmit }) => (
      <Dialog
        actionText='Create'
        title='Create Ledger'
        onSubmit={handleSubmit}
        onClose={() => {
          handleReset();
          onClose();
        }}
        open={isOpen}
      >
        <LedgerForm />
      </Dialog>
    )}
  />
);

PureCreateLedgerDialog.propTypes = {
  isOpen: PropTypes.bool.isRequired,
  onClose: PropTypes.func.isRequired,
  onCreate: PropTypes.func.isRequired,
};

const mapStateToProps = state => ({
  isOpen: state.ledgers.isCreateOpen,
});

const mapDispatchToProps = dispatch => ({
  onCreate: ledger => dispatch(createLedger(ledger)),
});

export default connect(mapStateToProps, mapDispatchToProps)(PureCreateLedgerDialog);
