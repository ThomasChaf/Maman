# Maman
A web crawler that determine the category of some website.

Each content is analysed and a score is calculated for each website and for each category. Categories are defined in the dictionary.json file.
Then the list of the website is sorted by score and category and displayed.

## Requirements ##

#### Modulary ####

* A C library.

https://github.com/ThomasChaf/Modulary

### Gumbo parser ###

* An html parser that create a dom

https://github.com/google/gumbo-parser

### Curl ###

* Finaly unused but keep in the purpose of crawling website

## Usage ##

### Configuration ###

#### dictionary.json ####

This file is used to store some list of words.

```json
{
    "categorie_1" : [
        ["WORD_1", "TYPE_1", "SCORE_1"],
        ["WORD_2", "TYPE_2", "SCORE_2"]
    ],
    "categorie_2" : [
        ["WORD_1", "TYPE_1", "SCORE_1"],
        ["WORD_2", "TYPE_2", "SCORE_2"]
    ]
}
```

#### types ####

Each word has a type.
* SIMPLE - *(default one)*
* NOUN
* VERB

The type is used to improve the analyse. A verb is conjugate. A noun is matched with his plural form.

### Crawling ###

The phantom-app is a sub app used to crawl all the website. It used a file containing a list of website as first argument.

    phantomjs crawler.js websites.txt

Then the html of the website is saved in a sub folder. This is this html that will be parsed by the maman application.

### Launch ###

-> With the list of websites:

    ./maman phantom-app/websites.txt

(OR)

-> With a single website *(to make a test)*

    ./maman test.html "Description"

### Results ###

```
========================================================
                    categorie_1
========================================================
[score_total_1]		http://www.website_1.com
[score_total_2]		http://www.website_2.com
========================================================
                    categorie_2
========================================================
[score_total_1]		http://www.website_1.com
[score_total_2]		http://www.website_2.com
========================================================
```

## Use case

This project was used on an application containing links. The purpose of Maman was to determine which links could be trustable. Two dictionaries were defined.
The first in contradiction. The second in relation with the supposed content.
It has displayed first dangerous links that redirect to undesired content. Then it has displayed down website. This has been usefull to clean the application.

## Credits

  - [Thomas Chafiol](http://github.com/ThomasChaf)
