import React from 'react';
import PropTypes from 'prop-types';
import AppBar from '@material-ui/core/AppBar';
import Button from '@material-ui/core/Button';
import CloseIcon from '@material-ui/icons/Close';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogTitle from '@material-ui/core/DialogTitle';
import IconButton from '@material-ui/core/IconButton';
import Slide from '@material-ui/core/Slide';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import withMobileDialog from '@material-ui/core/withMobileDialog';
import LedgerForm from './LedgerForm';

function Transition(props) {
  return <Slide direction='up' {...props} />;
}

const LedgerFormDialog = ({
  actionText,
  fullScreen,
  onClose,
  onSubmit,
  open,
  title,
}) => (
  <Dialog
    fullScreen={fullScreen}
    open={open}
    onClose={onClose}
    TransitionComponent={Transition}
  >
    {!fullScreen && <DialogTitle id='ledger-form-dialog'>{title}</DialogTitle>}
    {fullScreen && (
      <AppBar style={{ position: 'relative' }}>
        <Toolbar>
          <IconButton color='inherit' onClick={onClose}>
            <CloseIcon />
          </IconButton>
          <Typography variant='h6' color='inherit' style={{ flex: 1 }}>
            {title}
          </Typography>
          <Button color='inherit' type='submit' form='ledger-form'>
            {actionText}
          </Button>
        </Toolbar>
      </AppBar>
    )}

    <DialogContent>
      <LedgerForm onSubmit={onSubmit} />
    </DialogContent>

    {!fullScreen && (
      <DialogActions>
        <Button color='primary' onClick={onClose}>
          Cancel
        </Button>
        <Button color='primary' type='submit' form='ledger-form'>
          {actionText}
        </Button>
      </DialogActions>
    )}
  </Dialog>
);

LedgerFormDialog.propTypes = {
  fullScreen: PropTypes.bool.isRequired,
  open: PropTypes.bool.isRequired,
  onClose: PropTypes.func.isRequired,
  title: PropTypes.string.isRequired,
  actionText: PropTypes.string.isRequired,
  onSubmit: PropTypes.func.isRequired,
};

export default withMobileDialog()(LedgerFormDialog);
