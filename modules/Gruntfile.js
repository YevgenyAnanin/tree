module.exports = function (grunt) {
  grunt.initConfig({
    uglify: {
      extra_video_widget: {
        files: [{
          expand: true,
          cwd: 'video_widget/video_widget_includes/js',
          src: ['**/*.js'/*, '!fastclick.js'*/],
          dest: 'video_widget/video_widget_includes/js',
          ext: '.min.js',
          extDot: 'first'
        }]
      },
      video_widget: {
        options: {
          beautify: true,
          mangle: true
        },
        files: {
          'video_widget/video_widget_includes/angular/app.min.js': 'video_widget/video_widget_includes/angular/app.js',
          'video_widget/video_widget_includes/angular/videoGrid.min.js': 'video_widget/video_widget_includes/angular/videoGrid.js'
        }
      },
      leadership_angular: {
        files: [{
          expand: true,
          cwd: 'leadership/angular/js',
          src: '**/*.js',
          dest: 'leadership/angular/js',
          ext: '.min.js',
          extDot: 'first'
        }]
      }
    }
  });

  grunt.loadNpmTasks('grunt-contrib-uglify');

  grunt.registerTask('default', ['uglify', 'uglify:video_widget', 'uglify:extra_video_widget', 'uglify:leadership_angular']);
}