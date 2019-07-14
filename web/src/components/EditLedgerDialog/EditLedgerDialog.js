import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Formik } from 'formik';
import * as yup from 'yup';
import Dialog from '../Dialog/Dialog';
import LedgerForm from '../LedgerForm/LedgerForm';
import { hideModifyLedger, makeGetModifyLedger, modifyLedger } from '../../state/ducks/ledgers';

const schema = yup.object().shape({
  name: yup.string().max(128, 'Too long').required('Required'),
  defaultCurrency: yup.string().required('Required'),
});

export const PureEditLedgerDialog = ({
  isOpen,
  ledger,
  onClose,
  onUpdate,
}) => (
  <Formik
    initialValues={ledger}
    enableReinitialize
    validationSchema={schema}
    onSubmit={onUpdate}
    render={({ handleReset, handleSubmit }) => (
      <Dialog
        actionText='Save'
        title='Modify Ledger'
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

PureEditLedgerDialog.propTypes = {
  isOpen: PropTypes.func.isRequired,
  ledger: PropTypes.shape({
    name: PropTypes.string,
    defaultCurrency: PropTypes.string,
  }).isRequired,
  onClose: PropTypes.func.isRequired,
  onUpdate: PropTypes.func.isRequired,
};

const getModifyLedger = makeGetModifyLedger();

const mapStateToProps = state => ({
  isOpen: state.ledgers.isModifyOpen,
  ledger: getModifyLedger(state),
});

const mapDispatchToProps = dispatch => ({
  onClose: () => dispatch(hideModifyLedger()),
  onUpdate: ledger => dispatch(modifyLedger(ledger)),
});

export default connect(mapStateToProps, mapDispatchToProps)(PureEditLedgerDialog);
