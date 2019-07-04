import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import CurrencyInputField from './CurrencyInputField';

storiesOf('Input|CurrencyInputField', module)
  .add('default', () => (
    <CurrencyInputField />
  ))
  .add('with helperText', () => (
    <CurrencyInputField helperText='Type of money' />
  ))
  .add('with fullWidth=false', () => (
    <CurrencyInputField fullWidth={false} />
  ))
  .add('with label', () => (
    <CurrencyInputField label='Money type' />
  ))
  .add('with required=false', () => (
    <CurrencyInputField required={false} />
  ))
  .add('with field', () => (
    <CurrencyInputField field={{ value: 'UAH', onChange: action('onChange') }} />
  ));
