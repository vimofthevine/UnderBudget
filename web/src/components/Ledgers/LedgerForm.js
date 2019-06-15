import React from 'react';
import PropTypes from 'prop-types';
import coinify from 'coinify';
import MenuItem from '@material-ui/core/MenuItem';
import TextField from '@material-ui/core/TextField';
import withStyles from '@material-ui/core/styles/withStyles';

const styles = theme => ({
  submit: {
    marginTop: theme.spacing.unit * 3,
  },
});

class LedgerForm extends React.Component {
  constructor(props) {
    super(props);
    if (props.ledger) {
      this.state = {
        id: props.ledger.id,
        name: props.ledger.name,
        currency: props.ledger.defaultCurrency,
      };
    } else {
      this.state = {
        id: null,
        name: '',
        currency: 'USD',
      };
    }
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(e) {
    this.setState({
      [e.target.name]: e.target.value,
    });
  }

  handleSubmit(e) {
    e.preventDefault();
    const { onSubmit } = this.props;
    const { id, name, currency } = this.state;
    if (onSubmit) {
      onSubmit({
        id,
        name,
        defaultCurrency: currency,
      });
    }
  }

  render() {
    const { currency, name } = this.state;
    return (
      <form id='ledger-form' onSubmit={this.handleSubmit}>
        <TextField
          id='name'
          name='name'
          label='Name'
          margin='normal'
          autoFocus
          required
          fullWidth
          onChange={this.handleChange}
          value={name}
        />
        <TextField
          id='currency'
          name='currency'
          label='Default currency'
          helperText='Default currency to use for accounts and envelopes in the ledger'
          margin='normal'
          required
          select
          fullWidth
          onChange={this.handleChange}
          value={currency}
        >
          {Object.keys(coinify.currencies).map(code => (
            <MenuItem key={code} value={code}>
              {coinify.name(code)}
            </MenuItem>
          ))}
        </TextField>
      </form>
    );
  }
}

LedgerForm.propTypes = {
  ledger: PropTypes.object,
  onSubmit: PropTypes.func,
};

LedgerForm.defaultProps = {
  ledger: null,
  onSubmit: null,
};

export default withStyles(styles)(LedgerForm);
