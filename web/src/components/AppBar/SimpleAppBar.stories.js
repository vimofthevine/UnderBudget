import React from 'react';
import { storiesOf } from '@storybook/react';
import SimpleAppBar from './SimpleAppBar';

storiesOf('Layout|Header/AppBar', module)
  .add('simple', () => (
    <SimpleAppBar />
  ))
  .add('simple with title', () => (
    <SimpleAppBar title='App Title' />
  ));
