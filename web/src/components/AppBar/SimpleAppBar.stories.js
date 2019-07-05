import React from 'react';
import { storiesOf } from '@storybook/react';
import SimpleAppBar from './SimpleAppBar';

storiesOf('Layout|AppBar', module)
  .add('simple', () => (
    <SimpleAppBar />
  ))
  .add('simple with title', () => (
    <SimpleAppBar title='App Title' />
  ));
