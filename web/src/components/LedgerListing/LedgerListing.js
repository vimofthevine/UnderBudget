import React, { Fragment, useState } from 'react';
import PropTypes from 'prop-types';
import IconButton from '@material-ui/core/IconButton';
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

const LedgerListing = ({
  ledgers,
  onArchive,
  onEdit,
  onSelect,
  selectedLedger,
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
                    <IconButton onClick={() => onEdit(ledger.id)}>
                      <EditIcon />
                    </IconButton>
                  </Tooltip>
                  <Tooltip title='Archive ledger'>
                    <IconButton onClick={() => onArchive(ledger.id)}>
                      <ArchiveIcon />
                    </IconButton>
                  </Tooltip>
                  <Tooltip title='Select ledger'>
                    <IconButton
                      disabled={selectedLedger === ledger.id}
                      onClick={() => onSelect(ledger.id)}
                    >
                      <CheckIcon />
                    </IconButton>
                  </Tooltip>
                </TableCell>
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
    defaultCurrency: PropTypes.string,
    created: PropTypes.string,
    owner: PropTypes.shape({
      id: PropTypes.string,
      name: PropTypes.string,
    }),
  })).isRequired,
  onArchive: PropTypes.func.isRequired,
  onEdit: PropTypes.func.isRequired,
  onSelect: PropTypes.func.isRequired,
  selectedLedger: PropTypes.string.isRequired,
};

export default LedgerListing;
