import React from 'react'
import Button from '@material-ui/core/Button'
import LedgerFormDialog from './LedgerFormDialog'
import LedgerService from '../../services/LedgerService'

class Ledgers extends React.Component {
  constructor() {
    super()
    this.state = {
      formOpen : false
    }
    this.hideForm = this.hideForm.bind(this)
    this.showForm = this.showForm.bind(this)
    this.submitForm = this.submitForm.bind(this)
    this.service = new LedgerService()
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
    
  submitForm(ledger) {
    if (ledger.id) {
      
    } else {
      this.service.createLedger(ledger.name, ledger.defaultCurrency)
    }
    this.hideForm()
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
          onSubmit={this.submitForm}
        />
      </React.Fragment>
    )
  }
}

export default Ledgers