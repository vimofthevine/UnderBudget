import React from 'react';
import coinify from 'coinify';
import MenuItem from '@material-ui/core/MenuItem';
import TextField from '@material-ui/core/TextField';

const CurrencyInputField = props => (
  <TextField
    margin='normal'
    required
    select
    fullWidth
    {...props}
  >
    {Object.keys(coinify.currencies).map(code => (
      <MenuItem key={code} value={code}>
        {coinify.name(code)}
      </MenuItem>
    ))}
  </TextField>
);

CurrencyInputField.defaultProps = {
  id: 'currency',
  name: 'currency',
  label: 'Currency',
  value: 'USD',
};

export default CurrencyInputField;
