import React from 'react';
import PropTypes from 'prop-types';
import coinify from 'coinify';
import MenuItem from '@material-ui/core/MenuItem';
import TextField from '@material-ui/core/TextField';

const CurrencyInputField = ({ field, ...props }) => (
  <TextField
    margin='normal'
    required
    fullWidth
    select
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
};

CurrencyInputField.defaultProps = {
  field: null,
};

export default CurrencyInputField;
