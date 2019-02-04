import React from 'react'
import PropTypes from 'prop-types'
import AppBar from '@material-ui/core/AppBar'
import Button from '@material-ui/core/Button'
import CloseIcon from '@material-ui/icons/Close'
import Dialog from '@material-ui/core/Dialog'
import DialogActions from '@material-ui/core/DialogActions'
import DialogContent from '@material-ui/core/DialogContent'
import DialogTitle from '@material-ui/core/DialogTitle'
import IconButton from '@material-ui/core/IconButton'
import Slide from '@material-ui/core/Slide'
import Toolbar from '@material-ui/core/Toolbar'
import Typography from '@material-ui/core/Typography'
import withMobileDialog from '@material-ui/core/withMobileDialog'
import LedgerForm from './LedgerForm'

function Transition(props) {
  return <Slide direction="up" {...props} />
}

class LedgerFormDialog extends React.Component {
  render() {
    return (
      <Dialog
        fullScreen={this.props.fullScreen}
        open={this.props.open}
        onClose={this.props.onClose}
        TransitionComponent={Transition}
      >
        {!this.props.fullScreen &&
          <DialogTitle id="ledger-form-dialog">{this.props.title}</DialogTitle>
        }
        {this.props.fullScreen &&
          <AppBar style={{position: "relative"}}>
            <Toolbar>
              <IconButton color="inherit" onClick={this.props.onClose}>
                <CloseIcon />
              </IconButton>
              <Typography variant="h6" color="inherit" style={{flex: 1}}>
                {this.props.title}
              </Typography>
              <Button color="inherit" type="submit" form="ledger-form">
                {this.props.actionText}
              </Button>
            </Toolbar>
          </AppBar>
        }
        
        <DialogContent>
          <LedgerForm />
        </DialogContent>
        
        {!this.props.fullScreen &&
          <DialogActions>
            <Button color="primary" onClick={this.props.onClose}>
              Cancel
            </Button>
            <Button color="primary" type="submit" form="ledger-form">
              {this.props.actionText}
            </Button>
          </DialogActions>
        }
      </Dialog>
    )
  }
}

LedgerFormDialog.propTypes = {
  fullScreen: PropTypes.bool.isRequired,
  open: PropTypes.bool.isRequired,
  onClose: PropTypes.func.isRequired,
  title: PropTypes.string.isRequired,
  actionText: PropTypes.string.isRequired
}

export default withMobileDialog()(LedgerFormDialog)