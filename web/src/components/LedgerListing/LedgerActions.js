import React, { Fragment, useState } from 'react';
import PropTypes from 'prop-types';
import {
  ClickAwayListener,
  IconButton,
  Menu,
  MenuItem,
  Tooltip,
} from '@material-ui/core';
import ActionsIcon from '@material-ui/icons/MoreVert';
import ArchiveIcon from '@material-ui/icons/Archive';
import CheckIcon from '@material-ui/icons/Check';
import EditIcon from '@material-ui/icons/Edit';
import UnarchiveIcon from '@material-ui/icons/Unarchive';

const LedgerActions = ({
  isMobile,
  ledger,
  onArchive,
  onEdit,
  onSelect,
  onUnarchive,
  selectedLedgerId,
}) => {
  const [anchor, setAnchor] = useState(null);
  const handleClick = event => setAnchor(event.currentTarget);
  const handleClose = () => setAnchor(null);

  return (
    <Fragment>
      {!isMobile && (
        <Fragment>
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
        </Fragment>
      )}
      {isMobile && (
        <ClickAwayListener onClickAway={handleClose}>
          <Fragment>
            <IconButton onClick={handleClick}>
              <ActionsIcon />
            </IconButton>
            <Menu
              anchorEl={anchor}
              onClose={handleClose}
              open={Boolean(anchor)}
            >
              <MenuItem
                disabled={selectedLedgerId === ledger.id}
                onClick={() => onSelect(ledger)}
              >
                Select
              </MenuItem>
              <MenuItem onClick={() => onEdit(ledger)}>Edit</MenuItem>
              {ledger.archived && (
                <MenuItem onClick={() => onUnarchive(ledger)}>Unarchive</MenuItem>
              )}
              {!ledger.archived && (
                <MenuItem onClick={() => onArchive(ledger)}>Archive</MenuItem>
              )}
            </Menu>
          </Fragment>
        </ClickAwayListener>
      )}
    </Fragment>
  );
};

LedgerActions.propTypes = {
  isMobile: PropTypes.bool.isRequired,
  ledger: PropTypes.shape({
    id: PropTypes.string,
    name: PropTypes.string,
    defaultCurrency: PropTypes.string,
    archived: PropTypes.bool,
    created: PropTypes.string,
    owner: PropTypes.shape({
      id: PropTypes.string,
      name: PropTypes.string,
    }),
  }).isRequired,
  onArchive: PropTypes.func.isRequired,
  onEdit: PropTypes.func.isRequired,
  onSelect: PropTypes.func.isRequired,
  onUnarchive: PropTypes.func.isRequired,
  selectedLedgerId: PropTypes.string.isRequired,
};

export default LedgerActions;
