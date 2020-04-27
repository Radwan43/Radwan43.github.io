# My personal Blog
minimal blog theme built for Jekyll. The blog theme features a masonry grid, endless scrolling, and page transitions.


## Initial Setup
* [Installation](#installation)
* [Update Settings](#update-settings)
* [Create Posts](#create-posts)
* [Create Pages](#create-pages)
* [Create Navigation](#create-navigation)

## Customization
* [Contact Form](#contact-form)
* [Social Media Links](#social-media-links)
* [Disqus Comments](#disqus-comments)

## Additional Development
* [Deployment](#deployment)
* [Source Code](#source-code)
* [Donations](#donations)
* [Support](#support)

### Installation
Jekyll requires all dependencies to be saved in the ````Gemfile````. Run ````bundle install```` (Install [Bundler](http://bundler.io/) if it is not already) on your command line after downloading or cloning the theme. You can then run ````bundle exec jekyll serve```` or ````npm start```` to see your development site. Run ````bundle exec jekyll build```` or ````npm run build```` to build a production ready site for deployment.

### Update Settings
Almost everything to personalize your site is in the ````_config.yml````. 

```
# Site/SEO settings
email: okay@samesies.io
baseurl: ""
permalink: /:year/:month/:day/:title/
google_analytics: 

name: Thomas Vaeth
title: The Barber Theme
description: >
  Barber is a blog theme for Jekyll built by Thomas Vaeth for Samesies using HTML, Sass, and JavaScript.
url: http://barber.samesies.io
twitter_username: thomasvaeth
default_img: /assets/images/seo.jpg
social:
  - name: twitter
    url: https://twitter.com/thomasvaeth
  - name: instagram
    url: https://www.instagram.com/thomas.vaeth/
  - name: linkedin
    url: https://www.linkedin.com/in/thomasvaeth/
  - name: github
    url: https://github.com/samesies
  - name: codepen
    url: https://codepen.io/thomasvaeth/

# Contact settings
contact_img: /assets/images/placeholder-28.jpg
formcarry: https://formcarry.com/s/HkIo0nMb7

# Disqus settings
disqus: test-apkdzgmqhj

# MailChimp settings
mailchimp_action: https://samesies.us17.list-manage.com/subscribe/post-json?u=66ddf555dab480e6a8606430b&amp;id=89b3ee034f
mailchimp_input: b_66ddf555dab480e6a8606430b_89b3ee034f

# Author settings
author:
  - name: Thomas Vaeth
    bio: Thomas Vaeth was born in New York, raised in Pennsylvania, and transplanted in Washington. He was a Web Developer at Urban Influence, but now he's a Software Engineer at Getty Images.
    url: http://thomasvaeth.com

# Pagination settings
pagination:
  enabled: true
  debug: false
  per_page: 12
  permalink: '/page/:num/'
  title: ':title'
  limit: 0
  sort_field: 'date'
  sort_reverse: true
autopages:
  enabled: true
  categories:
    enabled: false
  collections:
    enabled: false
  tags:
    layouts: 
      - 'tag.html'
    title: 'The Barber Theme'
    permalink: '/tag/:tag'
    slugify:
      mode: raw
      cased: true
```

You can change the URL the [contact form](#contact-form) is sent to, add Google Analytics, change the SEO settings, grow your website with additional authors, and much more.

### Create Posts
All posts go upder the ````_posts```` directory. You can also have a ````_drafts```` directory with posts that will on your development page, but not in production.

```
---
layout: post
title: "Brunch Swag"
date: 2017-02-18
description: 
image: /assets/images/placeholder-15.jpg
author: Thomas Vaeth
tags: 
  - XOXO
  - La Croix
---
```

The front matter has to have a layout of page. All the other fields are completely optional. If you have an ````author```` variable, then it must match an author's name in ````_config.yml```` (see [Update Settings](#update-settings)). The ````tag```` variable will add a related section to the post and popular tags to the footer.

### Create Pages
Creating a static page is the same as creating a post. The only difference is a page is in the root of the directory rather than the ````_posts```` directory.

```
---
layout: page
title: Style Guide
image: /assets/images/placeholder-18.jpg
---
```

You just have to make sure the front matter has a layout of page instead of post. If there is no title or image, then the page will default to the site configuration.

### Create Navigation
You can create a navigation in ````_includes/navigation.html````. Visitors can be linked directly to pages right on the top of your website.

***

### Contact Form
The form uses [Formcarry](https://formcarry.com/) to send submitted messages straight to your inbox. The image on the popup is the the ````contact_img```` variable and the URL the forms sends to is the ````formcarry```` variable in ````_config.yml```` (see [Update Settings](#update-settings)).

![Contact Form](http://samesies.io/assets/images/barber/doc/framed-contact-form.jpg "Contact Form")

This file can be found in ````_includes/formscarry.html````. You can change the labels of the form here. After everything is set you will need to submit a message to yourself to confirm everything is correct.

### Social Media Links
[Font Awesome](http://fontawesome.io/) is used for the social media icons. The icons in the theme can be found in ````_includes/share.html```` and ````_includes/social.html````. The icons in ````_includes/share.html```` do not need to be edited unless you want to remove a certain website; however, the ones in ````_includes/social.html```` do have to be changed. You can follow the example that has been provided in ````_config.yml```` for you to link to all of your social media accounts  (see [Update Settings](#update-settings)). The naming convention has not changed from the instructions provided on Font Awesome.

### Disqus Comments
Comments can be enabled on every blog post in a few steps steps. The first step is to register your website with [Disqus](https://disqus.com/). Disqus will provide you with a shortname that you need for the next step. Once you have that the second step is to replace the ````disqus```` variable in ````_config.yml```` (see [Update Settings](#update-settings)). The third step is to open ````_includes/disqus.html```` and remove all the instructions. The final step is to visit a blog post and verify that your comments are there.

***

### Deployment
GitHub Pages [does not support]((https://help.github.com/articles/adding-jekyll-plugins-to-a-github-pages-site/)) custom plugins. The tag list and tag pagination are built using custom plugins. There are several options to avoid any errors while deploying to production.
* Run ````bundle exec jekyll build```` or ````npm run build```` and manually add the contents of the ```_site``` folder to the ```gh-pages``` branch.
* Link the repository to [Netlify](https://www.netlify.com/). Netlify will then rebuild the theme every time a commit is pushed to the repo.
* Finish setting up the [s3-website](https://github.com/klaemo/s3-website) package that is already included in the theme. This would deploy the theme to AWS S3 when ```npm run deploy``` is run.
