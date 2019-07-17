import React, { Fragment, useState } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import LinearProgress from '@material-ui/core/LinearProgress';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableHead from '@material-ui/core/TableHead';
import TablePagination from '@material-ui/core/TablePagination';
import TableRow from '@material-ui/core/TableRow';
import useMediaQuery from '@material-ui/core/useMediaQuery';
import { useTheme } from '@material-ui/styles';
import {
  makeGetLedgers,
  modifyLedger,
  selectLedger,
  showModifyLedger,
} from '../../state/ducks/ledgers';
import LedgerActions from './LedgerActions';

export const PureLedgerListing = ({
  isLoading,
  ledgers,
  ...props
}) => {
  const [page, setPage] = useState(0);
  const [rowsPerPage, setRowsPerPage] = useState(5);

  const handleChangePage = (event, newPage) => setPage(newPage);
  const handleChangeRowsPerPage = (event) => {
    setRowsPerPage(+event.target.value);
    setPage(0);
  };

  const theme = useTheme();
  const mobile = useMediaQuery(theme.breakpoints.down('sm'));

  return (
    <Fragment>
      <Table size={mobile ? 'small' : 'medium'}>
        <TableHead>
          <TableRow>
            <TableCell>Ledger</TableCell>
            <TableCell>Default Currency</TableCell>
            {!mobile && (
              <Fragment>
                <TableCell>Created</TableCell>
                <TableCell>Owner</TableCell>
              </Fragment>
            )}
            <TableCell>Actions</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {ledgers.slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
            .map(ledger => (
              <TableRow key={ledger.id}>
                <TableCell component='th' scope='row'>{ledger.name}</TableCell>
                <TableCell>{ledger.defaultCurrency}</TableCell>
                {!mobile && (
                  <Fragment>
                    <TableCell>{ledger.created}</TableCell>
                    <TableCell>{ledger.owner.name}</TableCell>
                  </Fragment>
                )}
                <TableCell>
                  <LedgerActions isMobile={mobile} ledger={ledger} {...props} />
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

const mapStateToProps = state => ({
  isLoading: state.ledgers.isLoading,
  ledgers: getLedgers(state),
  selectedLedgerId: state.ledgers.selectedLedgerId,
});

const mapDispatchToProps = dispatch => ({
  onArchive: ledger => dispatch(modifyLedger({ ...ledger, archived: true })),
  onEdit: ledger => dispatch(showModifyLedger(ledger)),
  onSelect: ledger => dispatch(selectLedger(ledger)),
  onUnarchive: ledger => dispatch(modifyLedger({ ...ledger, archived: false })),
});

export default connect(mapStateToProps, mapDispatchToProps)(PureLedgerListing);
