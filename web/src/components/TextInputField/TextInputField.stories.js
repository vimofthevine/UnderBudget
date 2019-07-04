import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import TextInputField from './TextInputField';

storiesOf('Input|TextInputField', module)
  .add('default', () => (
    <TextInputField />
  ))
  .add('with helperText', () => (
    <TextInputField helperText='Type of money' />
  ))
  .add('with fullWidth=false', () => (
    <TextInputField fullWidth={false} />
  ))
  .add('with label', () => (
    <TextInputField label='Message' />
  ))
  .add('with required=false', () => (
    <TextInputField required={false} />
  ))
  .add('with field', () => (
    <TextInputField field={{ value: 'hi there', onChange: action('onChange') }} />
  ));
