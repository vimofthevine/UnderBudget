import React from 'react';
import PropTypes from 'prop-types';
import AppBar from '@material-ui/core/AppBar';
import Button from '@material-ui/core/Button';
import MuiDialog from '@material-ui/core/Dialog';
import CircularProgress from '@material-ui/core/CircularProgress';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogTitle from '@material-ui/core/DialogTitle';
import IconButton from '@material-ui/core/IconButton';
import Slide from '@material-ui/core/Slide';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import withMobileDialog from '@material-ui/core/withMobileDialog';
import CloseIcon from '@material-ui/icons/Close';

const Transition = props => (
  <Slide direction='up' {...props} />
);

const Dialog = ({
  actionText,
  children,
  fullScreen,
  onClose,
  onSubmit,
  open,
  pending,
  title,
}) => (
  <MuiDialog
    fullScreen={fullScreen}
    open={open}
    onClose={onClose}
    TransitionComponent={Transition}
  >
    {!fullScreen && <DialogTitle>{title}</DialogTitle>}
    {fullScreen && (
      <AppBar style={{ position: 'relative' }}>
        <Toolbar>
          <IconButton color='inherit' onClick={onClose}>
            <CloseIcon />
          </IconButton>
          <Typography variant='h6' color='inherit' style={{ flex: 1 }}>
            {title}
          </Typography>
          <Button color='inherit' disabled={pending} onClick={onSubmit}>
            {actionText}
          </Button>
        </Toolbar>
      </AppBar>
    )}

    <DialogContent>
      {children}
    </DialogContent>

    {!fullScreen && (
      <DialogActions>
        <Button color='primary' onClick={onClose}>Cancel</Button>
        <Button color='primary' disabled={pending} onClick={onSubmit}>
          {pending && <CircularProgress />}
          {!pending && actionText}
        </Button>
      </DialogActions>
    )}
  </MuiDialog>
);

Dialog.propTypes = {
  actionText: PropTypes.string.isRequired,
  children: PropTypes.element.isRequired,
  fullScreen: PropTypes.bool.isRequired,
  onClose: PropTypes.func.isRequired,
  onSubmit: PropTypes.func.isRequired,
  open: PropTypes.bool.isRequired,
  pending: PropTypes.bool,
  title: PropTypes.string.isRequired,
};

Dialog.defaultProps = {
  pending: false,
};

export default withMobileDialog()(Dialog);
