import React from 'react';
import PropTypes from 'prop-types';
import coinify from 'coinify';
import MenuItem from '@material-ui/core/MenuItem';
import TextField from '@material-ui/core/TextField';

const CurrencyInputField = ({ field, form, helperText, ...props }) => (
  <TextField
    margin='normal'
    required
    fullWidth
    select
    error={form.errors[field.name]}
    helperText={form.errors[field.name] || helperText}
    {...field}
    {...props}
  >
    {Object.keys(coinify.currencies).map(code => (
      <MenuItem key={code} value={code}>
        {coinify.name(code)}
      </MenuItem>
    ))}
  </TextField>
);

CurrencyInputField.propTypes = {
  field: PropTypes.object,
  form: PropTypes.object,
  helperText: null,
};

CurrencyInputField.defaultProps = {
  field: { name: '' },
  form: { errors: {} },
  helperText: null,
};

export default CurrencyInputField;
