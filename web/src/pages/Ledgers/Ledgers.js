import React, { Fragment, useEffect, useState } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import Button from '@material-ui/core/Button';
import Checkbox from '@material-ui/core/Checkbox';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Snackbar from '@material-ui/core/Snackbar';
import CreateLedgerDialog from '../../components/CreateLedgerDialog/CreateLedgerDialog';
import EditLedgerDialog from '../../components/EditLedgerDialog/EditLedgerDialog';
import LedgerListing from '../../components/LedgerListing/LedgerListing';
import { dismissLedgerError, fetchLedgers, showCreateLedger } from '../../state/ducks/ledgers';

const Ledgers = ({
  ledgerError,
  onDismissError,
  onFetch,
  onShowCreate,
}) => {
  const [showArchived, setShowArchived] = useState(false);
  const handleChangeShowArchive = event => setShowArchived(event.target.checked);

  useEffect(() => {
    onFetch({ archived: showArchived });
  });

  return (
    <Fragment>
      <div style={{ display: 'flex' }}>
        <Button
          color='primary'
          onClick={onShowCreate}
          variant='contained'
        >
          Create new ledger
        </Button>
        <FormControlLabel
          control={
            <Checkbox checked={showArchived} onChange={handleChangeShowArchive} />
          }
          label='Show archived'
          style={{ marginLeft: 'auto' }}
        />
      </div>
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
  onFetch: PropTypes.func.isRequired,
  onShowCreate: PropTypes.func.isRequired,
};

const mapStateToProps = state => ({
  ledgerError: state.ledgers.error,
});

const mapDispatchToProps = dispatch => ({
  onDismissError: () => dispatch(dismissLedgerError()),
  onFetch: query => dispatch(fetchLedgers(query)),
  onShowCreate: () => dispatch(showCreateLedger()),
});

export default connect(mapStateToProps, mapDispatchToProps)(Ledgers);
