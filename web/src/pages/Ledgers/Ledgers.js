import React, { Fragment, useEffect, useState } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import Button from '@material-ui/core/Button';
import Snackbar from '@material-ui/core/Snackbar';
import CreateLedgerDialog from '../../components/CreateLedgerDialog/CreateLedgerDialog';
import EditLedgerDialog from '../../components/EditLedgerDialog/EditLedgerDialog';
import LedgerListing from '../../components/LedgerListing/LedgerListing';
import { showCreateLedger, dismissLedgerError } from '../../state/ducks/ledgers';

const Ledgers = ({
  ledgerError,
  onDismissError,
  onShowCreate,
}) => {
  return (
    <Fragment>
      <Button
        color='primary'
        onClick={onShowCreate}
        variant='contained'
      >
        Create new ledger
      </Button>
      <LedgerListing />
      <CreateLedgerDialog />
      <EditLedgerDialog />
      <Snackbar
        autoHideDuration={3000}
        message={ledgerError}
        onClose={onDismissError}
        open={ledgerError}
      />
    </Fragment>
  );
};

Ledgers.propTypes = {
  ledgerError: PropTypes.string.isRequired,
  onDismissError: PropTypes.func.isRequired,
  onShowCreate: PropTypes.func.isRequired,
};

const mapStateToProps = state => ({
  ledgerError: state.ledgers.error,
});

const mapDispatchToProps = dispatch => ({
  onDismissError: () => dispatch(dismissLedgerError()),
  onShowCreate: () => dispatch(showCreateLedger()),
});

export default connect(mapStateToProps, mapDispatchToProps)(Ledgers);
