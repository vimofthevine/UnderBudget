import React, { Fragment, useState } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import IconButton from '@material-ui/core/IconButton';
import LinearProgress from '@material-ui/core/LinearProgress';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableHead from '@material-ui/core/TableHead';
import TablePagination from '@material-ui/core/TablePagination';
import TableRow from '@material-ui/core/TableRow';
import Tooltip from '@material-ui/core/Tooltip';
import ArchiveIcon from '@material-ui/icons/Archive';
import CheckIcon from '@material-ui/icons/Check';
import EditIcon from '@material-ui/icons/Edit';
import UnarchiveIcon from '@material-ui/icons/Unarchive';
import { makeGetLedgers, selectLedger } from '../../state/ducks/ledgers';

export const PureLedgerListing = ({
  isLoading,
  ledgers,
  onArchive,
  onEdit,
  onSelect,
  onUnarchive,
  selectedLedgerId,
}) => {
  const [page, setPage] = useState(0);
  const [rowsPerPage, setRowsPerPage] = useState(5);

  const handleChangePage = (event, newPage) => setPage(newPage);
  const handleChangeRowsPerPage = (event) => {
    setRowsPerPage(+event.target.value);
    setPage(0);
  };

  return (
    <Fragment>
      <Table>
        <TableHead>
          <TableRow>
            <TableCell>Ledger</TableCell>
            <TableCell>Default Currency</TableCell>
            <TableCell>Created</TableCell>
            <TableCell>Owner</TableCell>
            <TableCell>Actions</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {ledgers.slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
            .map(ledger => (
              <TableRow key={ledger.id}>
                <TableCell component='th' scope='row'>{ledger.name}</TableCell>
                <TableCell>{ledger.defaultCurrency}</TableCell>
                <TableCell>{ledger.created}</TableCell>
                <TableCell>{ledger.owner.name}</TableCell>
                <TableCell>
                  <Tooltip title='Edit ledger'>
                    <IconButton onClick={() => onEdit(ledger)}>
                      <EditIcon />
                    </IconButton>
                  </Tooltip>
                  {ledger.archived && (
                    <Tooltip title='Unarchive ledger'>
                      <IconButton onClick={() => onUnarchive(ledger)}>
                        <UnarchiveIcon />
                      </IconButton>
                    </Tooltip>
                  )}
                  {!ledger.archived && (
                    <Tooltip title='Archive ledger'>
                      <IconButton onClick={() => onArchive(ledger)}>
                        <ArchiveIcon />
                      </IconButton>
                    </Tooltip>
                  )}
                  <Tooltip title='Select ledger'>
                    <IconButton
                      disabled={selectedLedgerId === ledger.id}
                      onClick={() => onSelect(ledger)}
                    >
                      <CheckIcon />
                    </IconButton>
                  </Tooltip>
                </TableCell>
              </TableRow>
            ))}
        </TableBody>
      </Table>
      {isLoading && <LinearProgress />}
      {ledgers.length > 5 && (
        <TablePagination
          component='div'
          labelRowsPerPage='Ledgers per page:'
          rowsPerPageOptions={[5, 15, 25]}
          count={ledgers.length}
          rowsPerPage={rowsPerPage}
          page={page}
          onChangePage={handleChangePage}
          onChangeRowsPerPage={handleChangeRowsPerPage}
        />
      )}
    </Fragment>
  );
};

PureLedgerListing.propTypes = {
  isLoading: PropTypes.bool.isRequired,
  ledgers: PropTypes.arrayOf(PropTypes.shape({
    id: PropTypes.string,
    name: PropTypes.string,
    defaultCurrency: PropTypes.string,
    archived: PropTypes.bool,
    created: PropTypes.string,
    owner: PropTypes.shape({
      id: PropTypes.string,
      name: PropTypes.string,
    }),
  })).isRequired,
  onArchive: PropTypes.func.isRequired,
  onEdit: PropTypes.func.isRequired,
  onSelect: PropTypes.func.isRequired,
  onUnarchive: PropTypes.func.isRequired,
  selectedLedgerId: PropTypes.string.isRequired,
};

const getLedgers = makeGetLedgers();

const mapStateToProps = (state, props) => ({
  isLoading: state.ledgers.isLoading,
  ledgers: getLedgers(state, props),
  selectedLedgerId: state.ledgers.selectedLedgerId,
});

const mapDispatchToProps = dispatch => ({
  onSelect: ledger => dispatch(selectLedger(ledger)),
});

export default connect(mapStateToProps, mapDispatchToProps)(PureLedgerListing);
