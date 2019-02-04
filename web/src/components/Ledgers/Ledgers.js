import React from 'react'
import Button from '@material-ui/core/Button'
import LedgerFormDialog from './LedgerFormDialog'

class Ledgers extends React.Component {
  constructor() {
    super()
    this.state = {
      formOpen : false
    }
    this.hideForm = this.hideForm.bind(this)
    this.showForm = this.showForm.bind(this)
  }
    
  hideForm() {
    this.setState({
      formOpen: false
    })
  }
    
  showForm() {
    this.setState({
      formOpen: true
    })
  }
    
  render() {
    return (
      <React.Fragment>
        <Button
          color="primary"
          onClick={this.showForm}
        >
          Create new ledger
        </Button>
        <LedgerFormDialog
          title="Create ledger"
          actionText="Save"
          open={this.state.formOpen}
          onClose={this.hideForm}
        />
      </React.Fragment>
    )
  }
}

export default Ledgers