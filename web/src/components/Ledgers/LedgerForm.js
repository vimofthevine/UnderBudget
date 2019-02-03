import React from 'react'
import PropTypes from 'prop-types'
import coinify from 'coinify'
import Button from '@material-ui/core/Button'
import MenuItem from '@material-ui/core/MenuItem'
import TextField from '@material-ui/core/TextField'
import withStyles from '@material-ui/core/styles/withStyles'

const styles = theme => ({
  submit: {
    marginTop: theme.spacing.unit * 3
  }
})

class LedgerForm extends React.Component {
  constructor(props) {
    super(props)
    if (props.ledger) {
      this.state = {
        id : props.ledger.id,
        name : props.ledger.name,
        currency : props.ledger.defaultCurrency
      }
    } else {
      this.state = {
        id: null,
        name: '',
        currency: 'USD'
      }
    }
    this.handleChange = this.handleChange.bind(this)
    this.handleSubmit = this.handleSubmit.bind(this)
  }
    
  handleChange(e) {
    this.setState({
      [e.target.name]: e.target.value
    })
  }
    
  handleSubmit(e) {
    e.preventDefault()
    if (this.props.onSubmit) {
      this.props.onSubmit({
        id: this.state.id,
        name: this.state.name,
        defaultCurrency: this.state.currency
      })
    }
  }
    
  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <TextField
          id="name"
          name="name"
          label="Name"
          margin="normal"
          autoFocus={true}
          required
          fullWidth
          onChange={this.handleChange}
          value={this.state.name}
        />
        <TextField
          id="currency"
          name="currency"
          label="Default currency"
          helperText="Default currency to use for accounts and envelopes in the ledger"
          margin="normal"
          required
          select
          fullWidth
          onChange={this.handleChange}
          value={this.state.currency}
        >
          {Object.keys(coinify.currencies).map(code => (
            <MenuItem key={code} value={code}>
              {coinify.name(code)}
            </MenuItem>
          ))}
        </TextField>
        <Button
          type="submit"
          variant="contained"
          color="primary"
          fullWidth
          className={this.props.classes.submit}
        >
          {this.props.actionText}
        </Button>
      </form>
    )
  }
}

LedgerForm.defaultProps = {
  actionText: "Create ledger"
}

LedgerForm.propTypes = {
  actionText: PropTypes.string.isRequired,
  ledger: PropTypes.object,
  onSubmit: PropTypes.func
}
        
export default withStyles(styles)(LedgerForm)