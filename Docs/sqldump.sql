--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'LATIN1';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: logs; Type: TABLE; Schema: public; Owner: SLive; Tablespace: 
--

CREATE TABLE logs (
    id uuid NOT NULL,
    server_ip character varying NOT NULL,
    type smallint,
    log character varying(100),
    datetime character varying(10)
);


ALTER TABLE public.logs OWNER TO "SLive";

--
-- Name: logs_pkey; Type: CONSTRAINT; Schema: public; Owner: SLive; Tablespace: 
--

ALTER TABLE ONLY logs
    ADD CONSTRAINT logs_pkey PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--
