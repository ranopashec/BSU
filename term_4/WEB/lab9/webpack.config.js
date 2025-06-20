const path = require('path');

module.exports = {
    entry: './public/app.js',
    output: {
        filename: 'bundle.js',
        path: path.resolve(__dirname, 'public', 'dist'),
        publicPath: '/dist/'
    },
    module: {
        rules: [
            {
                test: /\.css$/i,
                use: ['style-loader', 'css-loader'],
            },
            {
                test: /\.js$/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                    options: { presets: ['@babel/preset-env'] }
                }
            }
        ]
    },
    mode: 'development',
    devtool: 'source-map'
};
