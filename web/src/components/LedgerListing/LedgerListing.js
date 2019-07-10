import React, { Fragment, useState } from 'react';
import PropTypes from 'prop-types';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableHead from '@material-ui/core/TableHead';
import TablePagination from '@material-ui/core/TablePagination';
import TableRow from '@material-ui/core/TableRow';

const LedgerListing = ({ ledgers }) => {
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
          </TableRow>
        </TableHead>
        <TableBody>
          {ledgers.slice(page * rowsPerPage, page * rowsPerPage + rowsPerPage)
            .map(ledger => (
              <TableRow key={ledger.id}>
                <TableCell component='th' scope='row'>{ledger.name}</TableCell>
                <TableCell>{ledger.currency}</TableCell>
                <TableCell>{ledger.created}</TableCell>
                <TableCell>{ledger.owner.name}</TableCell>
              </TableRow>
            ))}
        </TableBody>
      </Table>
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

LedgerListing.propTypes = {
  ledgers: PropTypes.arrayOf(PropTypes.shape({
    id: PropTypes.string,
    name: PropTypes.string,
    currency: PropTypes.string,
    created: PropTypes.string,
    owner: PropTypes.shape({
      id: PropTypes.string,
      name: PropTypes.string,
    }),
  })).isRequired,
};

export default LedgerListing;
